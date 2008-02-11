# coding: utf-8

from pychan import widgets
import fife

class DatasetSelector(object):
	def __init__(self,engine,map,selectNotify):
		self.engine = engine
		self.map = map
		self.notify = selectNotify
		
		self.buildGui()
	
	def buildGui(self):
		self.gui = widgets.Window(title="Dataset selector")
		hbox = widgets.HBox(parent=self.gui)
		self.gui.addChild(hbox)
		scrollArea = widgets.ScrollArea(parent=hbox,size=(120,300))
		hbox.addChild(scrollArea)
		self.datasets = widgets.ListBox(parent=scrollArea)
		scrollArea.content = self.datasets
		scrollArea = widgets.ScrollArea(parent=hbox,size=(120,300))
		hbox.addChild(scrollArea)
		self.instances = widgets.ListBox(parent=scrollArea)
		scrollArea.content = self.instances
		scrollArea = widgets.ScrollArea(parent=hbox, size=(120,300))
		hbox.addChild(scrollArea)
		self.preview = widgets.Icon()
		scrollArea.content = self.preview

		hbox = widgets.HBox(parent=self.gui)
		self.gui.addChild(hbox)
		hbox.addSpacer( widgets.Spacer() )
		closeButton = widgets.Button(parent=hbox,text="Close")
		hbox.addChild( closeButton )
		closeButton.capture(self.hide)
		
		self.datasets.capture(self.updateInstances)
		self.datasets.items = [dat.Id() for dat in self.map.getDatasets()]
		self.datasets.selected = 0
		self.updateInstances()
	
		self.instances.capture(self.instanceSelected)

	
	def updateInstances(self):
		datid = self.datasets.selected_item
		self.selected_dataset = self.engine.getModel().getMetaModel().getDatasets('id', datid)[0]
		self.instances.items  = [obj.Id() for obj in self.selected_dataset.getObjects()]
		if not self.instances.selected_item:
			self.instances.selected = 0
		self.instanceSelected()

	def instanceSelected(self):
		self.selected_instance = self.instances.selected_item
		object = self.selected_dataset.getObjects('id', self.selected_instance)[0]
		self.notify(object)
		self._refreshPreview(object)

	def _refreshPreview(self, object):
		visual = None
		
		try:
			visual = object.get2dGfxVisual()
		except:
			print 'Visual Selection created for type without a visual?'
			raise	

		index = visual.getStaticImageIndexByAngle(0)
		if index == -1:
			print 'Visual missing static image.'
			return

		image = fife.GuiImage(index, self.engine.getImagePool())
		self.preview.image = image
		self.gui.adaptLayout()
	
	def show(self):
		self.gui.show()
	def hide(self):
		self.gui.hide()
